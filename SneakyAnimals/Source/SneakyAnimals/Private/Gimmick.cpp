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

// Sets default values
AGimmick::AGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemObject = nullptr;
}

// Called when the game starts or when spawned
void AGimmick::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(itemObject)) {
		itemObject = GetDefaultItemObject();
	}
	
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
	int32 key;
	if (Cast<AWH_BookshelfGimmick>(this))
	{
		key = Cast<AWH_BookshelfGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<AWH_WitchCauldronGimmick>(this))
	{
		key = Cast<AWH_WitchCauldronGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<AWH_BroomstickGimmick>(this))
	{
		key = Cast<AWH_BroomstickGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<AWH_PotionGimmick>(this))
	{
		key = Cast<AWH_PotionGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_PeriscopeGimmick>(this))
	{
		key = Cast<ASM_PeriscopeGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_PressButtonGimmick>(this))
	{
		key = Cast<ASM_PressButtonGimmick>(this)->OnMyActive(ActivePlayer);
	}
	else if (Cast<ASM_ComputerGimmick>(this))
	{
		key = Cast<ASM_ComputerGimmick>(this)->OnMyActive(ActivePlayer);
	}

	return key;
}

void AGimmick::SetActiveType(int32 aType)
{
	activeType = aType;
}

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
	if (!itemObject)
	{
		itemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());
	}
}
