// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapDoor.generated.h"

UCLASS()
class SNEAKYANIMALS_API ATrapDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* base;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* trigger;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* object;

	UPROPERTY(EditDefaultsOnly)
	int32 roomType = 0;

	FTimerHandle openDoorT;

	FTimerHandle closeDoorT;

	float lerpTime;

	bool bIsOpen = false;

	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void CloseDoor();

	UFUNCTION()
	void AutoOpen(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
