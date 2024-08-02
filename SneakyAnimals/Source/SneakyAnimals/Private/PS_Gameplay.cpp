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

	//// 유저 프로필 다 불러왔으면 랜덤한 유저 넘버를 할당받자
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
