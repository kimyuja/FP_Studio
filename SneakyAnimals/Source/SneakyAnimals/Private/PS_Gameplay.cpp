// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Gameplay.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "SAGameStateBase.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

void APS_Gameplay::BeginPlay()
{
	Super::BeginPlay();
	ClientRPC_Init();
}

void APS_Gameplay::ClientRPC_Init_Implementation()
{
	Super::Load_Player_UserProfile();
}

void APS_Gameplay::ClientRPC_Init()
{
	ClientRPC_Init_Implementation();
}

void APS_Gameplay::OnRep_Player_UserProfile_OR()
{
	/*
	PS_Base�� Player_UserProfile ���� 
	FText Username; �����̸�
	UTexture2D* User_Avatar; ���� ������ �̹���
	�� �ִ�.
	*/

	//// ���� ������ �� �ҷ������� ������ ���� �ѹ��� �Ҵ����
	//if (HasAuthority())
	//{
	//	ASAGameStateBase* gs_gameplay = Cast<ASAGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	//	gs_gameplay->SetPlayerNum();
	//}
}

void APS_Gameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Gameplay, PlayerRandNum);
	DOREPLIFETIME(APS_Gameplay, PlayerShowNum);
}
