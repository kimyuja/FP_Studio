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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> moniter;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* moniterUI;

private:

	
	float lerpTime;
	FTimerHandle pressT;

	// ������
	void Electrocution(AActor* ActivePlayer);
	// ����������
	void SelfExplosion();
	// ������û
	void SOS();

	/*UFUNCTION()
	virtual void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
