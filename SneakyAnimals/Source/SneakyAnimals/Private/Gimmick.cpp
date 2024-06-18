// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick.h"

// Sets default values
AGimmick::AGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGimmick::OnMyActive(AActor* ActivePlayer)
{
	if (!bCanActive)
	{
		return;
	}
}

void AGimmick::SetActiveType(int32 aType)
{
	activeType = aType;
}
