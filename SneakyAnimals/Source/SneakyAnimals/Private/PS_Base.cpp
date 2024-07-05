// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Base.h"
#include "Net/UnrealNetwork.h"


FStructure_Player_Appearance APS_Base::Get_Player_Appearance()
{
	return Player_Appearance;
}

void APS_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Base, Player_Appearance);
}
