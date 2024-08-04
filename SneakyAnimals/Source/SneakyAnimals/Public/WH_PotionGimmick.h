// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "WH_PotionGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API AWH_PotionGimmick : public AGimmick
{
	GENERATED_BODY()


public:

	AWH_PotionGimmick();

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

	UObject* WorldContextObject;

private:

	float lerpTime;

	FTimerHandle boomT;

	// 점점 작아져서 본인 사망
	void Extincion(AActor* ActivePlayer);
	// 마신 사람이 터져서 전체 사망
	void SelfExplosion(AActor* ActivePlayer);
	// 잠긴 문을 열 수 있게 됨(능력 획득)
	void knowledgeinjection();

	/*UFUNCTION()
	void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

	
};
