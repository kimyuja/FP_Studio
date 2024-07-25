// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "BS_GoldBarGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ABS_GoldBarGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ABS_GoldBarGimmick();

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
	
	TArray<class ATestPlayer*> players;

	FTimerHandle goldT;

	class ATestPlayer* _target;

	// 손대면 경보울리고 랜덤한명 죽음 (랜덤한명)
	void Alarm(AActor* ActivePlayer);
	// 손이 미끄러져서 금괴에 맞았다.(혼자)
	void Golden(AActor* ActivePlayer);
	// 금괴가 비밀 스위치여서 비밀 문 열고 탈출(클리어)
	void DoorOpen();

};
