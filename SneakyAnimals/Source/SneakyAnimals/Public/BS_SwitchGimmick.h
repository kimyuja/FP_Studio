// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "BS_SwitchGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ABS_SwitchGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ABS_SwitchGimmick();

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

private:
	
	FTimerHandle dropT;

	class ATestPlayer* dropP;

	float deltatime;

	// ������ ������ �ͼ� ������(��ü)
	void DangerousGaurd(AActor* ActivePlayer);
	// ������ �ٴ��� ������ ��������.(ȥ��)
	void DropMe(AActor* ActivePlayer);
	// ������ �ڷ� ���ư���.(ȥ��)
	void ThrowMe(AActor* ActivePlayer);
	// ������ ���� �����ַ� �Դ�.
	void SafeGaurd();
	
};
