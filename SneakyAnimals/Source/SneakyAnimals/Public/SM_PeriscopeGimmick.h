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

	float lerpTime;

	FTimerHandle boomT;

	// ¿·∏¡∞Ê¿Ã µπæ∆∞°º≠ æÛ±º¿ª √∆¥Ÿ.
	void PeriscopeSpin(AActor* ActivePlayer);
	// ¿·∏¡∞Ê º’¿‚¿Ã∞° ∂≥æÓ¡Æº≠ πﬂ¿ª √∆¥Ÿ.
	void ByeHandle(AActor* ActivePlayer);
	// ¿·±‰ πÆ¿ª ø≠ ºˆ ¿÷∞‘ µ (¥…∑¬ »πµÊ)
	void HandleShake();

	UFUNCTION()
	void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
