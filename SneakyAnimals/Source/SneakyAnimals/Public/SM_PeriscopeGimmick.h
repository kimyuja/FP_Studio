// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SM_PeriscopeGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASM_PeriscopeGimmick : public AGimmick
{
	GENERATED_BODY()
public:

	ASM_PeriscopeGimmick();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector activeRange;

	UObject* WorldContextObject;

private:

	float lerpTime;

	FTimerHandle spinT;
	FTimerHandle clearSpinT;

	// 잠망경이 돌아가서 얼굴을 쳤다.
	void PeriscopeSpin(AActor* ActivePlayer);
	// 잠망경 손잡이가 떨어져서 발을 쳤다.
	void ByeHandle(AActor* ActivePlayer);
	// 멀리 보이는 배에게 잠망경을 흔들어 구조요청을 했다.
	void HandleShake();

	/*UFUNCTION()
	void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
