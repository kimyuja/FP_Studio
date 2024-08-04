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
	PS_Base의 Player_UserProfile 에는 
	FText Username; 유저이름
	UTexture2D* User_Avatar; 유저 프로필 이미지
	가 있다.
	*/

	// Username을 각각 player show num에 맞는 player 1, 2, 3, 4에게 할당해주어야 함 (투표 UI와 클리어 UI)

}
