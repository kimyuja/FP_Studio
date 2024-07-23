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

// JSON ���� ��� ���� ����
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UGI_SneakyAnimals::Init()
{
	Super::Init();

	LoadUserIndexMap();

	// ����ý��ۿ��� �����������̽� ��������ʹ�.
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
	// ���� ���� �˻��� ���� ���̸� �α׸� ����ϰ� ����
	if (sessionSearch.IsValid() && sessionSearch->SearchState == EOnlineAsyncTaskState::InProgress)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ignoring new search request because one is already in progress."));
		return;
	}

	// UI�� Finding...�� Ȱ��ȭ �ش޶�...
	if (OnMySessionSearchFinishedDelegate.IsBound())
	{
		OnMySessionSearchFinishedDelegate.Broadcast(true);
	}

	// �����������̽��� �̿��ؼ� ���� ã��ʹ�.
	sessionSearch = MakeShareable(new FOnlineSessionSearch);

	// �ʱ�ȭ ���� Ȯ���� ���� �α� �߰�
	if (!sessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Session search is not valid after allocation."));
		return;
	}
	// �˻� ������ �����ϰ� �ʹ�.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Near);

	// LAN ����
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	// �ִ� �˻� �� 
	sessionSearch->MaxSearchResults = 30;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());

}

void UGI_SneakyAnimals::OnFindSessionsComplete(bool bWasSuccessful)
{
	// ���� �����ߴٸ�
	if (bWasSuccessful)
	{
		// sessionSearch���� ������ ��������ʹ�. -> UI�� ǥ���ϰ�ʹ�.
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
	// ȣ��Ʈ�� IP, Port��ȣ�� �����ͼ� ClientTravel �ϰ�ʹ�.
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
	// �÷��̾�� MainMenu_Level ���� ������ ������ʹ�.
	auto* pc = GetWorld()->GetFirstPlayerController();
	FString url = TEXT("/Game/RTY/LEVELS/StartMap");
	pc->ClientTravel(url, TRAVEL_Absolute);
}

FString UGI_SneakyAnimals::StringBase64Encode(const FString& str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UGI_SneakyAnimals::StringBase64Decode(const FString& str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
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

	// 1. ��������Ʈ ���� �����ΰ�?
	set.bIsDedicated = false;
	// 2. �������� ��Ī�Ұ��ΰ�?
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";
	// 3. ��Ī�� �����Ұ��ΰ�? (<��>ģ���ʴ�θ� �Ұ��ΰ�?)
	set.bShouldAdvertise = true;
	// 4. Presence�� ����Ұ��ΰ�?
	set.bUsesPresence = true;
	// 5. ���ӵ��߿� �������ɿ���
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;
	// 6. ������ ���� ������ �ִ� ����
	set.NumPublicConnections = playerCount;
	// 7. Ŀ���� ���� : ���̸�, ȣ��Ʈ�̸�

	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	FUserProfileResult userProfile = UFL_General::Get_UserProfile();
	FString hostName = userProfile.S_UserProfile.Username.ToString();
	mySessionName = hostName;
	set.Set(FName("HOST_NAME"), StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. netID�� ��������ʹ�.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession : %s"), *roomName);
	// ���� ����� �޶�� ��û�ϰ�ʹ�.
	sessionInterface->CreateSession(*netID, FName(*roomName), set);
}