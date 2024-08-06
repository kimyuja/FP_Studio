// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SM_ComputerGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASM_ComputerGimmick : public AGimmick
{
	GENERATED_BODY()
	
public:

	ASM_ComputerGimmick();

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

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* planePanel1;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* planePanel2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Myactivetype;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector activeRange;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> moniter;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* moniterUI;

	UObject* WorldContextObject;

	FTimerHandle DelayTimerHandle;

private:

	
	float lerpTime;
	
	FTimerHandle pressT;

	class ATestPlayer* p;
	// 감전사
	void Electrocution(AActor* ActivePlayer);
	// 자폭시퀸스
	void SelfExplosion();
	// 구조요청
	void SOS();

	/*UFUNCTION()
	virtual void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
