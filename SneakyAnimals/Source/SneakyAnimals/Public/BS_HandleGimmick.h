// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "BS_HandleGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ABS_HandleGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ABS_HandleGimmick();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual int32 OnMyActive(AActor* ActivePlayer);
	
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* base;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* trigger;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* object;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* activeObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Myactivetype;

private:


	// 만지면 감전사(혼자)
	void Pikachu(AActor* ActivePlayer);
	// 경보울리고 다죽음(전체)
	void WarningAlarm();
	// 돌리면 금고 문열려서 탈출 (클리어)
	void DoorOpen();
};
