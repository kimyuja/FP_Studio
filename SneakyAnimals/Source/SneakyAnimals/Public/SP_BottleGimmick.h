// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SP_BottleGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASP_BottleGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASP_BottleGimmick();

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

	UObject* WorldContextObject;

private:
	
	class ATestPlayer* _target;

	FTimerHandle sonT;

	// ��� �Ѿ�����.(ȥ��)
	void BottleTrap(AActor* ActivePlayer);
	// �߷� á���� ���ư���.(ȥ��)
	void Son(AActor* ActivePlayer);
	// �ȿ� ���谡 �־���. (Ŭ����)
	void MasterKey(AActor* ActivePlayer);
};
