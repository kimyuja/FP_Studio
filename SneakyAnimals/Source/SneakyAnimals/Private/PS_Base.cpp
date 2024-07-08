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
		// Ŭ���̾�Ʈ �Ӹ� �ƴ϶� ���������� OnRep ȣ���Ϸ��� �Ʒ�ó�� �������� ȣ��������Ѵ�.
		OnRep_Player_Appearance();
	}
}

void APS_Base::OnRep_Player_Appearance()
{
	UE_LOG(LogTemp, Warning, TEXT("APS_Base OnRep_Player_Appearance"));
	// �ڽ� Ŭ�������� �������̵� �� OnRep_Player_Appearance_OR()�� ȣ���ؾ� ��.. �̰� �ǳ�?
	OnRep_Player_Appearance_OR();
}

/*
Repnotifies OnRep functions �� �ڽ� Ŭ�������� �������̵� �� �� ����. �׷��� �������̵� �� �� �ֵ��� OnRep_Player_Appearance_OR()�� �������.
(OR = Override)
OR ���̻�� Override�� �ظ��̴�.
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