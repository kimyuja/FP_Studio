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
	// 
}

// Called every frame
void AGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AGimmick::OnMyActive(AActor* ActivePlayer)
{
	if (!bCanActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Active"));
		return -1;
	}
	_key = 0;
	if (Cast<AWH_BookshelfGimmick>(this))
	{
		_key = Cast<AWH_BookshelfGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<AWH_WitchCauldronGimmick>(this))
	{
		_key = Cast<AWH_WitchCauldronGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<AWH_BroomstickGimmick>(this))
	{
		_key = Cast<AWH_BroomstickGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<AWH_PotionGimmick>(this))
	{
		_key = Cast<AWH_PotionGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_PeriscopeGimmick>(this))
	{
		_key = Cast<ASM_PeriscopeGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_PressButtonGimmick>(this))
	{
		_key = Cast<ASM_PressButtonGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_ComputerGimmick>(this))
	{
		_key = Cast<ASM_ComputerGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_WhistleGimmick>(this))
	{
		_key = Cast<ASM_WhistleGimmick>(this)->OnMyActive(ActivePlayer);
	}

	/*if (_key != 2)
	{
		gameState->SetDeathCountUp(Cast<ATestPlayer>(ActivePlayer)->playerNum);
		UE_LOG(LogTemp, Warning, TEXT("YOU DIE"));
	}*/
	//ServerRPC_OnMyActive(ActivePlayer);
	UE_LOG(LogTemp, Warning, TEXT("Auth"));

	//ServerRPC_OnMyActive(ActivePlayer);
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
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
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
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
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