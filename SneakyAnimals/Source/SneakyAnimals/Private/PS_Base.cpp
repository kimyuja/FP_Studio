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
	// Ŭ���̾�Ʈ �Ӹ� �ƴ϶� ���������� OnRep ȣ���Ϸ��� �Ʒ�ó�� �������� ȣ��������Ѵ�.
	OnRep_Player_Appearance();
}

void APS_Base::ServerRPC_Update_Player_UserProfile_Implementation(const FStructure_UserProfile _Player_UserProfile)
{
	// ���� ���⼭ �������� if (HasAuthority()) ���̱�
	Player_UserProfile = _Player_UserProfile;
	OnRep_Player_UserProfile();

}

void APS_Base::ServerRPC_Update_Player_ConnectionInfo_Implementation(const FStructure_PlayerConnectionInfo _Player_ConnectionInfo)
{
	// ���� ���⼭ �������� if (HasAuthority()) ���̱�
	Player_ConnectionInfo = _Player_ConnectionInfo;
	OnRep_Player_ConnectionInfo();

}

void APS_Base::ClientRPC_Init_Implementation()
{
	// �ڽ� Ŭ�������� �������̵� �� ��
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
	// log : client�� rty, server�� kyj -> rty�� kyj ������ �˰� �ؾ� ��
	FUserProfileResult result = UFL_General::Get_UserProfile();

	// KYJ Test
	// �ε��� 0�� ���̺� ����
	// json ���Ͽ���, 0�� �ε����� ������ �������� ��(�������� ����, �ƿ� ���� ���·� �����ϴ� �� ����Ʈ)

	// result�� �ִ� ���� ����ũ �ε����� ���̺� ���ӿ� �ٽ� ������
	// �ٽ� �����ϴ� �� server rpc�� �ϸ� ���������� Ŭ���̾�Ʈ�� ���̺� ������ ����! (����ũ �ε����� ����)
	// ���� ������ �̵� �� FUserProfileResult result = UFL_General::Get_UserProfile(); 0�� �ε���
	// 0�� �ε����� Ŭ���̾�Ʈ ������ ���� ������ ������ �װɷ� json ���� ����ũ �ε��� ã�� ���� appearance ������ ��
	
	// ���� ó��
	UGI_SneakyAnimals* GameInstance = Cast<UGI_SneakyAnimals>(GetGameInstance());

	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance or invalid cast to UGI_SneakyAnimals."));
		return;
	}

	FString Username = result.S_UserProfile.Username.ToString();
	int32 idx = -1;

	try
	{
		idx = GameInstance->GetUserIndex(Username);
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Exception occurred while getting user index: %s"), *FString(e.what()));
		return;
	}
	if (idx == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("User index not found for username: %s"), *Username);
		return;
	}

	//int32 idx = Cast<UGI_SneakyAnimals>(GetGameInstance())->GetUserIndex(result.S_UserProfile.Username.ToString());
	ServerRPC_Update_SaveGame_Player_UserProfile(idx, result.S_UserProfile);

	// idx == 0 �̸� ����
	//if (idx == 0 && !HasAuthority()) // server�� ����
	//if (idx == 0) // client -> server�� ����
	if (idx != GetWorld()->GetGameState()->PlayerArray.Num()-1 + Cast<UGI_SneakyAnimals>(GetGameInstance())->KickCount && HasAuthority()) // �� �Ǵµ� �����ϸ� ũ���� ��
	{
		UE_LOG(LogTemp, Warning, TEXT("Kickcount : %d"), Cast<UGI_SneakyAnimals>(GetGameInstance())->KickCount);
		// �ٽ� �� idx ���� �� ����
		FTimerHandle t;
		GetWorld()->GetTimerManager().SetTimer(t, [&]() {
			APS_Base::Load_Player_UserProfile();
			return;
			}, 0.2f, false);
	}
	else {
		// player state ���� �´� �ε������...
		if (HasAuthority())
		{
			// ������ �ε��� 0 ������
			// KYJ Test �̰� �غ��� �� �Ǹ� �ּ� ó�� �ϱ�
			result = UFL_General::Get_UserProfile_with_idx(0);
		} 
		else
		{
			// Ŭ���̾�Ʈ�� ���� �ε����� ������
			result = UFL_General::Get_UserProfile_with_idx(idx);
		}
		if (!result.S_UserProfile.Username.IsEmpty())
		{
			// �� player state ���� my name ����صα�
			SetPlayerName(result.S_UserProfile.Username.ToString());
		}
	}


	if (result.success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Player User Profile : %s"), *result.S_UserProfile.Username.ToString());
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
	// Loading Screen Texture2D ��� ����
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/Placeholders/T_ProfilePicture_Placeholder.T_ProfilePicture_Placeholder"));

	// Loading Screen Texture2D �ε�
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
	// �ڽ� Ŭ�������� �������̵� �� OnRep_Player_Appearance_OR()�� ȣ���ؾ� ��
	OnRep_Player_Appearance_OR();
}

void APS_Base::OnRep_Player_UserProfile()
{
	// �ڽ� Ŭ�������� �������̵� �� OnRep_Player_UserProfile_OR()�� ȣ���ؾ� ��
	OnRep_Player_UserProfile_OR();
}

void APS_Base::OnRep_Player_ConnectionInfo()
{
	// �ڽ� Ŭ�������� �������̵� �� OnRep_Player_ConnectionInfo_OR()�� ȣ���ؾ� ��
	OnRep_Player_ConnectionInfo_OR();
}


void APS_Base::OnRep_Player_Appearance_OR()
{
	/*
	Repnotifies OnRep functions �� �ڽ� Ŭ�������� �������̵� �� �� ����. �׷��� �������̵� �� �� �ֵ��� OnRep_Player_Appearance_OR()�� �������.
	(OR = Override)
	OR ���̻�� Override�� �ظ��̴�.
	*/
}

void APS_Base::OnRep_Player_UserProfile_OR()
{
	/*
	Repnotifies OnRep functions �� �ڽ� Ŭ�������� �������̵� �� �� ����. �׷��� �������̵� �� �� �ֵ��� OnRep_Player_UserProfile_OR()�� �������.
	(OR = Override)
	OR ���̻�� Override�� �ظ��̴�.
	*/
}

void APS_Base::OnRep_Player_ConnectionInfo_OR()
{
	/*
	Repnotifies OnRep functions �� �ڽ� Ŭ�������� �������̵� �� �� ����. �׷��� �������̵� �� �� �ֵ��� OnRep_Player_ConnectionInfo_OR()�� �������.
	(OR = Override)
	OR ���̻�� Override�� �ظ��̴�.
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