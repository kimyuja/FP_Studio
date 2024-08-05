// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SP_CartGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASP_CartGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASP_CartGimmick();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> cart;

	UObject* WorldContextObject;

private:

	float lerpTime;
	
	FTimerHandle roadRollerT;
	FTimerHandle doorT;

	class ATestPlayer* cartTarget;

	// īƮ �����̰� ������ �Ѿ����� (ȥ��)
	void Defective(AActor* ActivePlayer);
	// īƮ�� �ٸ�������� �������� (ȥ��)
	void RoadRoller(AActor* ActivePlayer);
	// īƮ�� �������� ���� �μ��� (Ŭ����)
	void RollingCart();
};
