// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "FL_General.h"


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
	FUserProfileResult result = UFL_General::Get_UserProfile();
	if (result.success)
	{
		// KYJ TEST
		Player_UserProfile = result.S_UserProfile;

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

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Base, Player_Appearance);
	DOREPLIFETIME(APS_Base, Player_UserProfile);
	DOREPLIFETIME(APS_Base, Player_ConnectionInfo);
}