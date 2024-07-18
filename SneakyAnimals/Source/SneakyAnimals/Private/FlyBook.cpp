// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyBook.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"

// Sets default values
AFlyBook::AFlyBook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void AFlyBook::BeginPlay()
{
	Super::BeginPlay();
	
	SearchRandomPlayer();

	FTimerHandle attackT;
	GetWorldTimerManager().SetTimer(attackT, [&]() { bAttack = true; }, 3.0, false, 0);
}

// Called every frame
void AFlyBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bAttack)
	{
		AttackRandomPlayer();
	}
}

void AFlyBook::SearchRandomPlayer()
{
	players.Empty();

	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		players.Add(*it);
	}

	targetNum = FMath::RandRange(0, players.Num() - 1);
}

void AFlyBook::AttackRandomPlayer()
{
	if (FVector::Dist(GetActorLocation(), players[targetNum]->GetActorLocation()) > 100.0f)
	{
		FVector targetLoc = (players[targetNum]->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), targetLoc.X, targetLoc.Y, targetLoc.Z);
		SetActorLocation(GetActorLocation() + targetLoc * 1.0f);
	}
	else
	{
		players[targetNum]->bIsDie = true;
		players[targetNum]->Respawn();
		players[targetNum]->DeathCounting();
		Destroy();
	}
}

