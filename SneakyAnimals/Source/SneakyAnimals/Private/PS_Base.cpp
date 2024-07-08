// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"
#include "Net/UnrealNetwork.h"


void APS_Base::ServerRPC_Update_Player_Appearance_Implementation(const FStructure_Player_Appearance _Player_Appearance)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("APS_Base ServerRPC_Update_Player_Appearance_Implementation Has Authority"));
		// Call OnRep_Player_Appearance
		Player_Appearance = _Player_Appearance;
		// 클라이언트 뿐만 아니라 서버에서도 OnRep 호출하려면 아래처럼 수동으로 호출해줘야한다.
		OnRep_Player_Appearance();
	}
}

void APS_Base::OnRep_Player_Appearance()
{
	UE_LOG(LogTemp, Warning, TEXT("APS_Base OnRep_Player_Appearance"));
	// 자식 클래스에서 오버라이드 된 OnRep_Player_Appearance_OR()를 호출해야 함.. 이게 되나?
	OnRep_Player_Appearance_OR();
}

/*
Repnotifies OnRep functions 는 자식 클래스에서 오버라이드 될 수 없다. 그래서 오버라이드 할 수 있도록 OnRep_Player_Appearance_OR()를 만들었다.
(OR = Override)
OR 접미사는 Override의 준말이다.
*/
void APS_Base::OnRep_Player_Appearance_OR()
{
	UE_LOG(LogTemp, Warning, TEXT("APS_Base OnRep_Player_Appearance_OR"));
}

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Base, Player_Appearance);
}