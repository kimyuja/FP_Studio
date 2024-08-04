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

	accel += DeltaTime * 10.0f;

	if(bAttack && ActivePlayer)
	{
		AttackActivePlayer();
	}
}

void AFlyBook::SearchRandomPlayer()
{
	/*players.Empty();

	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		players.Add(*it);
	}

	targetNum = FMath::RandRange(0, players.Num() - 1);*/
}


void AFlyBook::AttackActivePlayer()
{
	if (ActivePlayer && FVector::Dist(GetActorLocation(), ActivePlayer->GetActorLocation()) > 100.0f)
	{
		FVector targetLoc = (ActivePlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		SetActorLocation(GetActorLocation() + targetLoc * (1.0f + accel));
	}
	else if (ActivePlayer)
	{
		ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
		if (player)
		{
			player->bIsDie = true;
			player->Respawn();
			player->DeathCounting();
			Destroy();
		}
	}
}

void AFlyBook::SetActivePlayer(AActor* Player)
{
	ActivePlayer = Player;
}

