// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "WH_BroomstickGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API AWH_BroomstickGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	AWH_BroomstickGimmick();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


public:
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

private:
	
	float lerpTime = 0;

	FTimerHandle broomSmashT;



	// ���ڷ簡 �÷��̾ ������ ���
	void BroomSmash(AActor* ActivePlayer);
	// ���� �̼�
	void PoorDriver(AActor* ActivePlayer);
	// ���� ����
	void DoorBurst(AActor* ActivePlayer);

	/*UFUNCTION()
	void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
