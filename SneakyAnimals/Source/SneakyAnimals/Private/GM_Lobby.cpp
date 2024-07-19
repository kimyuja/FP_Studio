// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Lobby.h"
#include "PC_Base.h"
#include "PS_Lobby.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "GS_Lobby.h"

void AGM_Lobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APS_Lobby* ps_lobby = Cast<APS_Lobby>(NewPlayer->PlayerState);
	if (ps_lobby)
	{
		ps_lobby->bCanHostForceLaunchGame = bCanHostForceLaunchGame;
		//ps_lobby->onrep_bcanhostforcelaunchgame();
	}
}

void AGM_Lobby::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
}

void AGM_Lobby::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AGM_Lobby::ServerTravel()
{
	Create_MapTravel_LoadingScreens();
	FTimerHandle LoadingScreen_Duration;
	GetWorldTimerManager().SetTimer(LoadingScreen_Duration, [&]()
		{
			ServerTravel_ToGameplayMap();
		}, 1.0f, false);
}

void AGM_Lobby::Create_MapTravel_LoadingScreens()
{
	for (APlayerController* pc : All_PCs) {
		UE_LOG(LogTemp, Warning, TEXT("AGM_Lobby Create_MapTravel_LoadingScreens (All PCs)"));
		APC_Base* pc_base = Cast<APC_Base>(pc);
		pc_base->ClientRPC_Create_MapTravel_LoadingScreen();
	}
}

void AGM_Lobby::Handle_PlayerDisconnection()
{
	AGS_Lobby* gs_lobby = GetWorld()->GetGameState<AGS_Lobby>();
	if (gs_lobby)
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = gs_lobby;
		LatentInfo.ExecutionFunction = FName("Update_Lobby");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = FMath::Rand();
		
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.5f, LatentInfo);
	}
}
