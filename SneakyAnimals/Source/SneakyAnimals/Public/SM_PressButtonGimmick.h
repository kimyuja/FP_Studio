// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SM_PressButtonGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASM_PressButtonGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASM_PressButtonGimmick();

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

private:

	float lerpTime;
	FTimerHandle pressT;

	TArray<class ATestPlayer*> players;

	// 문 열려서 물 들어옴
	void Waterbomb();
	// 불이 꺼지고 랜덤 한명 사망
	void Blinklife();
	// 잠수함이 지상으로 올라간다.
	void Autopilot();

	/*UFUNCTION()
	void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	
};
