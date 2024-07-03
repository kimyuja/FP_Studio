// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shark.generated.h"

UCLASS()
class SNEAKYANIMALS_API AShark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector startLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector endLoc;

	FVector moveLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;
	
	bool bIsStart = false;

	UFUNCTION()
	void Jaws();


};
