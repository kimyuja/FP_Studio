// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_SneakyAnimals.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include <../../../../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include "Online/OnlineSessionNames.h"
#include <DataStructure.h>
#include <string>
#include "FL_General.h"
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

// JSON 관련 헤더 파일 포함
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UGI_SneakyAnimals::Init()
{
	Super::Init();

	LoadUserIndexMap();

	// 서브시스템에서 세션인터페이스 가져오고싶다.
	auto subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGI_SneakyAnimals::OnCreateSessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGI_SneakyAnimals::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGI_SneakyAnimals::OnJoinSessionComplete);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGI_SneakyAnimals::OnMyExitRoomComplete);
		}
	}
}

int32 UGI_SneakyAnimals::GetUserIndex(const FString& UserName)
{
	if (!UserIndexMap.Contains(UserName))
	{
		int32 NewIndex = UserIndexMap.Num();
		UserIndexMap.Add(UserName, NewIndex);
		SaveUserIndexMap();
	}
	MyName = FText::FromString(UserName);
	UE_LOG(LogTemp, Warning, TEXT("Get User Index UserName : %s"), *UserName);
	return UserIndexMap[UserName];
}

void UGI_SneakyAnimals::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
    {
        OnCreateSessionSuccess.Broadcast();
    }
    else
    {
        OnCreateSessionFailure.Broadcast();
    }
}

void UGI_SneakyAnimals::FindOtherSessions()
{
	// 현재 세션 검색이 진행 중이면 로그를 출력하고 종료
	if (sessionSearch.IsValid() && sessionSearch->SearchState == EOnlineAsyncTaskState::InProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ignoring new search request because one is already in progress."));
		return;
	}

	// UI의 Finding...을 활성화 해달라...
	if (OnMySessionSearchFinishedDelegate.IsBound())
	{
		OnMySessionSearchFinishedDelegate.Broadcast(true);
	}

	// 세션인터페이스를 이용해서 방을 찾고싶다.
	sessionSearch = MakeShareable(new FOnlineSessionSearch);

	// 초기화 여부 확인을 위해 로그 추가
	if (!sessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Session search is not valid after allocation."));
		return;
	}
	// 검색 조건을 설정하고 싶다.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Near);

	// LAN 여부
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// 최대 검색 수 
	sessionSearch->MaxSearchResults = 30;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());

}

void UGI_SneakyAnimals::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 만약 성공했다면
	if (bWasSuccessful)
	{
		// sessionSearch에서 정보를 가져오고싶다. -> UI로 표현하고싶다.
		if (!sessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Session search is not valid in OnFindSessionsComplete."));
			return;
		}
		auto results = sessionSearch->SearchResults;
		if (results.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete results.Num() == 0"));
		}
		for (int i = 0; i < results.Num(); i++)
		{
			auto item = results[i];
			if (false == item.IsValid()) { continue; }

			FSessionInfo info;
			info.Set(i, item);

			FString roomName;
			FString hostName;
			item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

			info.roomName = StringBase64Decode(roomName);
			info.hostName = StringBase64Decode(hostName);

			OnMySessionSearchCompleteDelegate.Broadcast(info);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString());
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete bWasSuccessful is false"));
	}

	if (OnMySessionSearchFinishedDelegate.IsBound())
	{
		OnMySessionSearchFinishedDelegate.Broadcast(false);
	}
}

void UGI_SneakyAnimals::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mySessionName), sessionSearch->SearchResults[index]);
}

void UGI_SneakyAnimals::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// 호스트의 IP, Port번호를 가져와서 ClientTravel 하고싶다.
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("ClientTravel URL : %s"), *url);

		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
	else 
	{
		// create pop up (Failed to join session)
	}
}

void UGI_SneakyAnimals::ExitRoom()
{
	sessionInterface->DestroySession(FName(*mySessionName));
}

void UGI_SneakyAnimals::OnMyExitRoomComplete(FName sessionName, bool bWasSuccessful)
{
	// 플레이어는 MainMenu_Level 으로 여행을 떠나고싶다.
	auto* pc = GetWorld()->GetFirstPlayerController();
	FString url = TEXT("/Game/RTY/LEVELS/StartMap");
	pc->ClientTravel(url, TRAVEL_Absolute);
}

FString UGI_SneakyAnimals::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UGI_SneakyAnimals::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}

void UGI_SneakyAnimals::GetRandomplayer()
{
	if (!ranPlayer)
	{
		for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
		{
			players.Add(*it);
		}
		int32 ranNum = FMath::RandRange(0, players.Num() - 1);
		ranPlayer = players[ranNum];
	}
}


void UGI_SneakyAnimals::LoadUserIndexMap()
{
	FString FilePath = FPaths::ProjectDir() / TEXT("UserIndex.json");
	FString JsonString;

	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			for (auto& Elem : JsonObject->Values)
			{
				UserIndexMap.Add(Elem.Key, Elem.Value->AsNumber());
			}
		}
	}
}

void UGI_SneakyAnimals::SaveUserIndexMap()
{
	FString FilePath = FPaths::ProjectDir() / TEXT("UserIndex.json");
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	for (auto& Elem : UserIndexMap)
	{
		JsonObject->SetNumberField(Elem.Key, Elem.Value);
	}

	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FFileHelper::SaveStringToFile(JsonString, *FilePath);
}

void UGI_SneakyAnimals::CreateSession(FString roomName, int32 playerCount)
{
	FOnlineSessionSettings set;

	// 1. 데디케이트 서버 접속인가?
	set.bIsDedicated = false;
	// 2. 랜선으로 매칭할것인가?
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";
	// 3. 매칭을 공개할것인가? (<ㅡ>친구초대로만 할것인가?)
	set.bShouldAdvertise = true;
	// 4. Presence를 사용할것인가?
	set.bUsesPresence = true;
	// 5. 게임도중에 참여가능여부
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;
	// 6. 참여할 공개 연결의 최대 갯수
	set.NumPublicConnections = playerCount;
	// 7. 커스텀 정보 : 방이름, 호스트이름

	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	FUserProfileResult userProfile = UFL_General::Get_UserProfile();
	FString hostName = userProfile.S_UserProfile.Username.ToString();
	mySessionName = hostName;
	set.Set(FName("HOST_NAME"), StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. netID를 가져오고싶다.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession : %s"), *roomName);
	// 방을 만들어 달라고 요청하고싶다.
	sessionInterface->CreateSession(*netID, FName(*roomName), set);
}