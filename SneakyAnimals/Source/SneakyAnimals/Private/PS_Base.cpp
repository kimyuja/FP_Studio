// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "FL_General.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include "GI_SneakyAnimals.h"
#include "GS_Lobby.h"


void APS_Base::ServerRPC_Update_Player_Appearance_Implementation(const FStructure_Player_Appearance _Player_Appearance)
{
	Player_Appearance = _Player_Appearance;
	// 클라이언트 뿐만 아니라 서버에서도 OnRep 호출하려면 아래처럼 수동으로 호출해줘야한다.
	OnRep_Player_Appearance();
}

void APS_Base::ServerRPC_Update_Player_UserProfile_Implementation(const FStructure_UserProfile _Player_UserProfile)
{
	// 만약 여기서 에러나면 if (HasAuthority()) 붙이기
	Player_UserProfile = _Player_UserProfile;
	OnRep_Player_UserProfile();

}

void APS_Base::ServerRPC_Update_Player_ConnectionInfo_Implementation(const FStructure_PlayerConnectionInfo _Player_ConnectionInfo)
{
	// 만약 여기서 에러나면 if (HasAuthority()) 붙이기
	Player_ConnectionInfo = _Player_ConnectionInfo;
	OnRep_Player_ConnectionInfo();

}

void APS_Base::ClientRPC_Init_Implementation()
{
	// 자식 클래스에서 오버라이드 할 것
}

void APS_Base::Load_Player_Appearance()
{
	FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
	if (result.bSuccess)
	{
		ServerRPC_Update_Player_Appearance_Implementation(result.Player_Appearance);
		return;
	}
	else
	{
		return;
	}

}

//void APS_Base::Load_Player_UserProfile()
//{
//	// 세이브 게임 0번 인덱스에서 유저 프로필을 불러옴
//	FUserProfileResult result = UFL_General::Get_UserProfile();
//
//	// json 파일에서, 0번 인덱스는 무조건 서버여야 함(유저네임 주의 fp studio : 0 이 서버)
//	// 예외 처리
//	UGameInstance* GI = GetGameInstance();
//	if (!IsValid(GI))
//	{
//		UE_LOG(LogTemp, Error, TEXT("GetGameInstance() returned nullptr."));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("GetGameInstance() returned valid instance."));
//	}
//	UGI_SneakyAnimals* GameInstance = Cast<UGI_SneakyAnimals>(GI);
//	if (GameInstance == nullptr)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance or invalid cast to UGI_SneakyAnimals."));
//		return;
//	}
//
//	FString Username = result.S_UserProfile.Username.ToString();
//	// 클라이언트일 때 유저네임이 맨끝 유저네임이 진짜 자기 꺼임
//	// 클라이언트면 맨 끝 유저네임 뽑을 때까지 계속 반복 (SG에 남아있는 예전 프로필은 안 돼)
//	// 맨 끝인지 확인하는 건 Username으로 getuserindex 함수 실행하고 그 값이 userindexmap num이랑 같은지 확인하면 됨
//	// 근데 그 인덱스 값은 서버랑 다름 주의!!
//	int32 idx = -1;
//	// 강퇴할 때 kick count 를 게임 인스턴스 말고 모두가 공유하는-서버 따로 클라 따로가 아닌 곳-에 저장해서 업뎃하면 됨
//	// kick count 는 ServerRPC_Update_KickCount(int32 cnt);
//
//	// 게임 맵으로 이동 후 클라 : json에 있는 가장 마지막 유저네임이 본인
//	// 게임 맵으로 이동 후 서버 : json에 있는 0번이 서버 꺼.
//
//	try
//	{
//		idx = GameInstance->Get_UserIndex(Username);
//		//// 클라이언트인데, 유저 프로필이 예전 save game에 남아있던 유저 프로필이라면...
//		//if (!HasAuthority() && idx != GameInstance->Get_MyUserIndex_Num())
//		//{
//		//	// 최신 유저 프로필로 될 때까지 계속 다시 해
//		//	FTimerHandle t;
//		//	GetWorld()->GetTimerManager().SetTimer(t, [&]() {
//		//		APS_Base::Load_Player_UserProfile();
//		//		return;
//		//		}, 0.2f, false);
//		//	return;
//		//}
//		//else if (!HasAuthority() && idx == GameInstance->Get_MyUserIndex_Num()) {
//		//	// 클라이언트인데, 최신 유저 프로필을 불러왔다면...
//
//		//	// 서버의 save game에 json 인덱스 끝에다가 저장해
//		//	ServerRPC_Update_SaveGame_Player_UserProfile(idx, result.S_UserProfile);
//		//}
//	}
//	catch (const std::exception& e)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Exception occurred while getting user index: %s"), *FString(e.what()));
//		return;
//	}
//	if (idx == -1)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("User index not found for username: %s"), *Username);
//		return;
//	}
//
//	//int32 idx = Cast<UGI_SneakyAnimals>(GetGameInstance())->GetUserIndex(result.S_UserProfile.Username.ToString());
//	ServerRPC_Update_SaveGame_Player_UserProfile(idx, result.S_UserProfile);
//
//	// idx == 0 이면 서버
//	//if (idx == 0 && !HasAuthority()) // server만 나옴
//	//if (idx == 0) // client -> server로 전염
//	if (idx < GetWorld()->GetGameState()->PlayerArray.Num()-1 + Cast<AGS_Lobby>(GetWorld()->GetGameState())->Get_KickCount() && HasAuthority()) // 잘 되는데 강퇴하고 같은 놈이 프로필 바꿔서 다시 들어오면 강퇴한 놈의 프로필로 들어감
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Kickcount : %d"), Cast<AGS_Lobby>(GetWorld()->GetGameState())->Get_KickCount());
//		// 다시 해 idx 맞을 때 까지
//		FTimerHandle t;
//		GetWorld()->GetTimerManager().SetTimer(t, [&]() {
//			APS_Base::Load_Player_UserProfile();
//			return;
//			}, 0.2f, false);
//	}
//	else {
//		// player state 에게 맞는 인덱스라면...
//		if (HasAuthority())
//		{
//			// 서버면 인덱스 0 가져와
//			// KYJ Test 이거 해보고 안 되면 주석 처리 하기
//			result = UFL_General::Get_UserProfile_with_idx(0);
//		} 
//		else
//		{
//			// 클라이언트면 고유 인덱스로 가져와
//			result = UFL_General::Get_UserProfile_with_idx(idx);
//		}
//		if (!result.S_UserProfile.Username.IsEmpty())
//		{
//			// 각 player state 별로 my name 기억해두기
//			SetPlayerName(result.S_UserProfile.Username.ToString());
//		}
//	}
//
//
//	if (result.success)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Load Player User Profile : %s"), *result.S_UserProfile.Username.ToString());
//
//		ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile);
//		////if (HasAuthority())
//		////{
//		////	FTimerHandle t;
//		////	GetWorld()->GetTimerManager().SetTimer(t, [&]() {
//		////		//ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile); // 방만들고 5초 뒤 터짐
//		////		//Load_Player_UserProfile();// 여전히 host 것만 나옴
//		////		}, 5.0f, false);
//		////}
//		////else {
//		////	ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile);
//		////}
//		//if (HasAuthority())
//		//{
//		//}
//		//else {
//		//	FTimerHandle t;
//		//	GetWorld()->GetTimerManager().SetTimer(t, [&]() {
//		//		//ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile); // 5초 뒤 클라이언트 터짐
//		//		ServerRPC_Update_Player_UserProfile_Implementation(UFL_General::Get_UserProfile().S_UserProfile);
//		//		// KYJ Test 0726 윗줄 해보고 안 되면 아랫줄 주석 해제 후 다시 해보기
//		//		GetWorld()->GetTimerManager().SetTimer(t, [&]() {
//
//		//		Load_Player_UserProfile();// 빈 프로필만 나옴 서버에서
//		//			}, 2.0f, false);
//		//		// 아랫줄 주석해제 후 안 되면 윗줄만 하고 Update_lobby()만 추가해서 해보기
//		//		
//		//		}, 5.0f, false);
//		//	//ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile);
//		//}
//
//		return;
//	}
//	else
//	{
//		FStructure_UserProfile tmp;
//		tmp.Username = FText::FromString(TEXT("Username"));
//		tmp.User_Avatar = T_ProfilePicture;
//		ServerRPC_Update_Player_UserProfile_Implementation(tmp);
//		return;
//	}
//}

void APS_Base::Load_Player_UserProfile()
{
	// 세이브 게임 0번 인덱스에서 유저 프로필을 불러옴
	FUserProfileResult result = UFL_General::Get_UserProfile();
	if (result.success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Player User Profile : %s"), *result.S_UserProfile.Username.ToString());
		if (!HasAuthority() && (result.S_UserProfile.Username.ToString() == "" || result.S_UserProfile.Username.ToString() == Cast<AGS_Lobby>(GetWorld()->GetGameState())->ServerName.ToString()))
		{
			//  클라이언트인데 아직 유저 프로필 제대로 못 불러왔으면 다시 불러 와
			FTimerHandle t;
			GetWorld()->GetTimerManager().SetTimer(t, [&]() {
				ServerRPC_Update_Player_UserProfile_Implementation(UFL_General::Get_UserProfile().S_UserProfile);
				Load_Player_UserProfile();
			}, 0.3f, false);
			return;
		}
		else if (HasAuthority() && Cast<AGS_Lobby>(GetWorld()->GetGameState())->PlayerArray.Num() > 1 && result.S_UserProfile.Username.ToString() == Cast<AGS_Lobby>(GetWorld()->GetGameState())->ServerName.ToString()) {
			// 서버이고 다른 사람 들어왔는데 서버 네임으로만 불러왔으면 다시 불러 와
			FTimerHandle t;
			GetWorld()->GetTimerManager().SetTimer(t, [&]() {
				Load_Player_UserProfile();
				}, 0.3f, false);
			return;
		}
		ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile);
		return;
	}
	else
	{
		FStructure_UserProfile tmp;
		tmp.Username = FText::FromString(TEXT("Username"));
		tmp.User_Avatar = T_ProfilePicture;
		ServerRPC_Update_Player_UserProfile_Implementation(tmp);
		return;
	}
}

void APS_Base::Load_Player_ConnectionInfo(bool ClientReadyStatus)
{
	// Sets Player Connection Info incase we are host
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		// The host is always ready, so set ready status to true.
		FStructure_PlayerConnectionInfo p;
		p.bIsHost = true;
		p.bReadyStatus = true;
		p.PlayerID = 0;
		ServerRPC_Update_Player_ConnectionInfo(p);
	}
	// Sets Player Connection Info incase we are a client
	else
	{
		FStructure_PlayerConnectionInfo p;
		p.bIsHost = false;
		// ClientReadyStatus : This variable gets set by the GM_Lobby variable called 'CanHostForceLaunch'
		p.bReadyStatus = ClientReadyStatus;
		p.PlayerID = 0;
		ServerRPC_Update_Player_ConnectionInfo(p);
	}

}

APS_Base::APS_Base()
{
	// Loading Screen Texture2D 경로 설정
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/Placeholders/T_ProfilePicture_Placeholder.T_ProfilePicture_Placeholder"));

	// Loading Screen Texture2D 로드
	T_ProfilePicture = Cast<UTexture2D>(TextureRef.TryLoad());
}

void APS_Base::BeginPlay()
{
	Super::BeginPlay();

	// This initialization event can be overridden in Child Player States
	ClientRPC_Init();
}

void APS_Base::OnRep_Player_Appearance()
{
	// 자식 클래스에서 오버라이드 된 OnRep_Player_Appearance_OR()를 호출해야 함
	OnRep_Player_Appearance_OR();
}

void APS_Base::OnRep_Player_UserProfile()
{
	// 자식 클래스에서 오버라이드 된 OnRep_Player_UserProfile_OR()를 호출해야 함
	OnRep_Player_UserProfile_OR();
}

void APS_Base::OnRep_Player_ConnectionInfo()
{
	// 자식 클래스에서 오버라이드 된 OnRep_Player_ConnectionInfo_OR()를 호출해야 함
	OnRep_Player_ConnectionInfo_OR();
}


void APS_Base::OnRep_Player_Appearance_OR()
{
	/*
	Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_Appearance_OR()를 만들었다.
	(OR = Override)
	OR 접미사는 Override의 준말이다.
	*/
}

void APS_Base::OnRep_Player_UserProfile_OR()
{
	/*
	Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_UserProfile_OR()를 만들었다.
	(OR = Override)
	OR 접미사는 Override의 준말이다.
	*/
}

void APS_Base::OnRep_Player_ConnectionInfo_OR()
{
	/*
	Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_ConnectionInfo_OR()를 만들었다.
	(OR = Override)
	OR 접미사는 Override의 준말이다.
	*/
}

void APS_Base::ServerRPC_Update_SaveGame_Player_UserProfile_Implementation(int32 uniqueIdx, const FStructure_UserProfile _Player_UserProfile)
{
	//UFL_General::Save_UserProfile_with_idx(Cast<UGI_SneakyAnimals>(GetGameInstance())->Get_UserIndex(_Player_UserProfile.Username.ToString()), _Player_UserProfile);
	UFL_General::Save_UserProfile_with_idx(uniqueIdx, _Player_UserProfile);
}

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Base, Player_Appearance);
	DOREPLIFETIME(APS_Base, Player_UserProfile);
	DOREPLIFETIME(APS_Base, Player_ConnectionInfo);
}