// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SM_WhistleGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASM_WhistleGimmick : public AGimmick
{
	GENERATED_BODY()

public:
	
	ASM_WhistleGimmick();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual UItemObject* GetDefaultItemObject() override;

	UPROPERTY(EditDefaultsOnly)
	class UItemObject* newItemObject;

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

	
	float lerpTime;
	
	FTimerHandle pressT;

	TArray<class ATestPlayer*> players;

	// 감전사
	void BabyShark();
	// 자폭시퀸스
	void IronSupplementation(AActor* ActivePlayer);
	// 구조요청
	void WhalePush();	
};
