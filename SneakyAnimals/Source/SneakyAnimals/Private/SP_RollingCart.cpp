// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_RollingCart.h"
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "GI_SneakyAnimals.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>


// Sets default values
ASP_RollingCart::ASP_RollingCart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	//SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASP_RollingCart::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority()) Cast<UGI_SneakyAnimals>(GetGameInstance())->GetRandomplayer();
	target = Cast<UGI_SneakyAnimals>(GetGameInstance())->ranPlayer;
	//UE_LOG(LogTemp, Warning, TEXT("3"));
	/*GetWorldTimerManager().SetTimer(roadRollerT, [&]()
		{
			FVector targetLoc = (Cast<UGI_SneakyAnimals>(GetGameInstance())->ranPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			UE_LOG(LogTemp, Warning, TEXT("Go! %s"), *target->GetName());
			SetActorLocation(GetActorLocation() + targetLoc * 1.0);
		}, 0.03f, true, 0);*/
}

// Called every frame
void ASP_RollingCart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!target)
	{
		target = Cast<UGI_SneakyAnimals>(GetGameInstance())->ranPlayer;
		return;
	}
	/*if (FVector::Dist(GetActorLocation(), target->GetActorLocation()) < 100.0)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Boom!!!!!!!!!!!"));
		GetWorldTimerManager().ClearTimer(roadRollerT);
		target->bIsDie = true;
		target->Respawn();
		target->DeathCounting();
		Destroy(true);
	}*/
	if (FVector::Dist(GetActorLocation(), target->GetActorLocation()) > 100.0f)
	{
		FVector targetLoc = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), targetLoc.X, targetLoc.Y, targetLoc.Z);
		SetActorLocation(GetActorLocation() + targetLoc * (1.0f));
	}
	else
	{
		target->bIsDie = true;
		target->Respawn();
		target->DeathCounting();
		Destroy();
	}
}

void ASP_RollingCart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASP_RollingCart, target);
}

