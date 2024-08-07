// Fill out your copyright notice in the Description page of Project Settings.


#include "Shark.h"

// Sets default values
AShark::AShark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
}

// Called when the game starts or when spawned
void AShark::BeginPlay()
{
	Super::BeginPlay();
	
	moveLoc = (endLoc - startLoc).GetSafeNormal();
}

// Called every frame
void AShark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		Jaws();
	}
}

void AShark::Jaws()
{
	if (!bIsStart)
	{
		bIsStart = true;
		SetActorLocation(startLoc);
	}
	if (FVector::Dist(GetActorLocation(), endLoc) < 50.0f)
	{
		bIsActive = false;
	}
	else
	{
		SetActorLocation(GetActorLocation() + moveLoc * 150.0f);
	}
}

