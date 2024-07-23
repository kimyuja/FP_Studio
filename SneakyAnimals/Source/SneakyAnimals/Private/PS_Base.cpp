// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "FL_General.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include "GI_SneakyAnimals.h"


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

void APS_Base::Load_Player_UserProfile()
{
	// log : client는 rty, server는 kyj -> rty를 kyj 서버가 알게 해야 함
	FUserProfileResult result = UFL_General::Get_UserProfile();

	// KYJ Test
	if (HasAuthority())
	{
		// 인덱스 0번 세이브 게임
		// json 파일에서, 0번 인덱스는 무조건 서버여야 함(유저네임 주의, 아예 없앤 상태로 시작하는 게 베스트)

	} 
	else
	{
		// result에 있는 값을 유니크 인덱스인 세이브 게임에 다시 저장함
		// 다시 저장하는 걸 server rpc로 하면 서버에서도 클라이언트의 세이브 게임이 보임! (유니크 인덱스로 접근)
		// 게임 맵으로 이동 후 FUserProfileResult result = UFL_General::Get_UserProfile(); 0번 인덱스
		// 0번 인덱스에 클라이언트 본인의 유저 네임이 있으니 그걸로 json 에서 유니크 인덱스 찾은 다음 appearance 입히면 됨
		int32 idx = Cast<UGI_SneakyAnimals>(GetGameInstance())->GetUserIndex(result.S_UserProfile.Username.ToString());
		ServerRPC_Update_SaveGame_Player_UserProfile(idx, result.S_UserProfile);
		UE_LOG(LogTemp, Warning, TEXT("SaveGame_Player_UserProfile %s idx: %d"), *result.S_UserProfile.Username.ToString(), idx);

		result = UFL_General::Get_UserProfile_with_idx(idx);
	}

	if (result.success)
	{
		ServerRPC_Update_Player_UserProfile_Implementation(result.S_UserProfile);
		UE_LOG(LogTemp, Warning, TEXT("Load Player User Profile : %s"), *result.S_UserProfile.Username.ToString());
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
	UFL_General::Save_UserProfile_with_idx(uniqueIdx, _Player_UserProfile);
}

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Base, Player_Appearance);
	DOREPLIFETIME(APS_Base, Player_UserProfile);
	DOREPLIFETIME(APS_Base, Player_ConnectionInfo);
}