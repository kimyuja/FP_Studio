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

	// Username�� ���� player show num�� �´� player 1, 2, 3, 4���� �Ҵ����־�� �� (��ǥ UI�� Ŭ���� UI)

}
