// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SP_BottleGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASP_BottleGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASP_BottleGimmick();

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
	
	class ATestPlayer* _target;

	FTimerHandle sonT;

	// 밟고 넘어졌다.(혼자)
	void BottleTrap(AActor* ActivePlayer);
	// 발로 찼더니 날아갔다.(랜덤 한명)
	void Son(AActor* ActivePlayer);
	// 안에 열쇠가 있었다. (클리어)
	void MasterKey(AActor* ActivePlayer);
};
