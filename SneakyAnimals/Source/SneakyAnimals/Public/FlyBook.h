// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyBook.generated.h"

UCLASS()
class SNEAKYANIMALS_API AFlyBook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyBook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class AActor* ActivePlayer;

	UFUNCTION()
	void SetActivePlayer(AActor* Player);

private:
	
	int32 targetNum;

	bool bAttack = false;

	float accel;

	UPROPERTY()
	TArray<class ATestPlayer*> players;


	UFUNCTION()
	void SearchRandomPlayer();

	UFUNCTION()
	void AttackActivePlayer();


};
