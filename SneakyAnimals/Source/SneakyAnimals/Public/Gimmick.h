// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick.generated.h"

UCLASS()
class SNEAKYANIMALS_API AGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimmick();

	UPROPERTY(EditAnywhere)
	bool bIsFinished = false;

	UPROPERTY(EditAnywhere)
	bool bCanActive = false;

	UPROPERTY(EditAnywhere)
	int32 activeType = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	int32 OnMyActive(AActor* ActivePlayer);

	UFUNCTION()
	void SetActiveType(int32 aType);
};
