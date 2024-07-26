// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "BS_LaserGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ABS_LaserGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ABS_LaserGimmick();

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

	// 만진사람 본인만 죽음( 혼자)
	void ElectricGrill(AActor* ActivePlayer);
	// 경보울려서 다죽음 (전체)
	void WarningDeath(AActor* ActivePlayer);
	// 가짜여서 그냥냥통과해서 클리어(클리어)
	void Faker(AActor* ActivePlayer);	
};
