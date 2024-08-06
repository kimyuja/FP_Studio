// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmick.h"
#include "ItemObject.h"
#include "WH_BookshelfGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "SM_PeriscopeGimmick.h"
#include "SM_PressButtonGimmick.h"
#include "SM_ComputerGimmick.h"
#include "TestPlayer.h"
#include "SM_WhistleGimmick.h"
#include "SAModeBase.h"
#include "SAGameInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "SAGameStateBase.h"
#include "SP_CartGimmick.h"
#include "SP_CleanerGimmick.h"
#include "SP_ShowcaseGimmick.h"
#include "SP_BottleGimmick.h"
#include "BS_HandleGimmick.h"
#include "BS_GoldBarGimmick.h"
#include "BS_SwitchGimmick.h"
#include "BS_LaserGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PS_Gameplay.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDieLogWidget.h"

// Sets default values
AGimmick::AGimmick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemObject = nullptr;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AGimmick::BeginPlay()
{
	Super::BeginPlay();

	gameState = Cast<ASAGameStateBase>(GetWorld()->GetGameState());


	// ¼±¹Î
	if (!IsValid(itemObject)) {
		itemObject = GetDefaultItemObject();
	}

	if (playerDieLogWidgetClass)
	{
		playerDieLogWidget = CreateWidget<UPlayerDieLogWidget>(GetWorld(), playerDieLogWidgetClass);

		if (playerDieLogWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("lingdingdong"));

			// playerDieLogWidget->AddToViewport();
		}
	}
	// 
}

// Called every frame
void AGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (timerCheck > 5.0)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		timerCheck = 0;
	}
}

int32 AGimmick::OnMyActive(AActor* ActivePlayer)
{
	if (!bCanActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Active"));
		return 3;
	}
	bCanActive = false;
	FText playerName = Cast<APS_Gameplay>(Cast<ATestPlayer>(ActivePlayer)->GetPlayerState())->Player_UserProfile.Username;
	//Cast<ATestPlayer>(ActivePlayer)->bIsDie = true;
	//Cast<ATestPlayer>(ActivePlayer)->bCanActive = false;
	for (TActorIterator<AGimmick> g(GetWorld()); g; ++g)
	{
		g->_key = 0;
	}
	if (Cast<AWH_BookshelfGimmick>(this))
	{
		_key = Cast<AWH_BookshelfGimmick>(this)->OnMyActive(ActivePlayer);
		
		playerDieLogWidget->GetLogSet(playerName, this, _key);
		// playerDieLogWidget->AddToViewport();
	}
	else if (Cast<AWH_WitchCauldronGimmick>(this))
	{
		_key = Cast<AWH_WitchCauldronGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<AWH_BroomstickGimmick>(this))
	{
		_key = Cast<AWH_BroomstickGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<AWH_PotionGimmick>(this))
	{
		_key = Cast<AWH_PotionGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASM_PeriscopeGimmick>(this))
	{
		_key = Cast<ASM_PeriscopeGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASM_PressButtonGimmick>(this))
	{
		_key = Cast<ASM_PressButtonGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASM_ComputerGimmick>(this))
	{
		_key = Cast<ASM_ComputerGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASM_WhistleGimmick>(this))
	{
		_key = Cast<ASM_WhistleGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASP_CartGimmick>(this))
	{
		_key = Cast<ASP_CartGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASP_CleanerGimmick>(this))
	{
		_key = Cast<ASP_CleanerGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASP_ShowcaseGimmick>(this))
	{
		_key = Cast<ASP_ShowcaseGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ASP_BottleGimmick>(this))
	{
		_key = Cast<ASP_BottleGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ABS_HandleGimmick>(this))
	{
		_key = Cast<ABS_HandleGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ABS_GoldBarGimmick>(this))
	{
		_key = Cast<ABS_GoldBarGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ABS_SwitchGimmick>(this))
	{
		_key = Cast<ABS_SwitchGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	else if (Cast<ABS_LaserGimmick>(this))
	{
		_key = Cast<ABS_LaserGimmick>(this)->OnMyActive(ActivePlayer);

		playerDieLogWidget->GetLogSet(playerName, this, _key);
	}
	if (_key == 2)
	{
		//Cast<ATestPlayer>(ActivePlayer)->ClearStage();
		for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
		{
			if(it->IsLocallyControlled())
			{
				it->ServerRPC_ClearStage();
				break;
			}
		}
	}
	else
	{
		playerDieLogWidget->AddToViewport();
	}
	/*if (soundNum == 0)
	{
		soundNum = _key;
	}
	if (sounds.Num() == 3 && Cast<ATestPlayer>(ActivePlayer)->IsLocallyControlled())
	{
		bIsPlayed = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), sounds[soundNum], ActivePlayer->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("Play Sound %d, %swwwwwww"), soundNum, *UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()));
	}*/
	//ServerRPC_OnMyActive(ActivePlayer);
	
	//ServerRPC_OnMyActive(ActivePlayer);
	/*FTimerHandle playT;
	GetWorldTimerManager().SetTimer(playT, [&](){
		bIsPlayed = false;
	}, 1.0, false, 0.5);*/

	return _key;
}

void AGimmick::SetActiveType(int32 aType)
{
	activeType = aType;
}

void AGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsFinished)
	{
		return;
	}
	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
	if (player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("overlap"));
		player->bCanActive = true;
		//OnMyActive(OtherActor);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}

void AGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
	if (player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("overlap"));
		player->bCanActive = false;
		player->g = nullptr;
		bCanActive = false;
		//OnMyActive(OtherActor);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("RTY"));
	}
}

void AGimmick::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGimmick, _key);
	DOREPLIFETIME(AGimmick, soundNum);
	DOREPLIFETIME(AGimmick, bIsPlayed);
}

// ¼±¹Î
UItemObject* AGimmick::GetDefaultItemObject()
{
	if (!itemObject)
	{
		InitializeItemObject();
	}

	return itemObject;
}

void AGimmick::InitializeItemObject()
{
	itemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());
}
// 