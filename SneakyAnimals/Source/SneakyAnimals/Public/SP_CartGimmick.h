// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SP_CartGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASP_CartGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASP_CartGimmick();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly)
	class UItemObject* newItemObject;

	virtual UItemObject* GetDefaultItemObject() override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector activeRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> cart;

	UObject* WorldContextObject;

private:

	float lerpTime;
	
	FTimerHandle roadRollerT;
	FTimerHandle doorT;

	class ATestPlayer* cartTarget;

	// 카트 손잡이가 빠져서 넘어졌다 (혼자)
	void Defective(AActor* ActivePlayer);
	// 카트가 다른사람에게 굴러갔다 (혼자)
	void RoadRoller(AActor* ActivePlayer);
	// 카트가 굴러가서 문을 부순다 (클리어)
	void RollingCart();
};
