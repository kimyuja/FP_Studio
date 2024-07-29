// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SP_ShowcaseGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASP_ShowcaseGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASP_ShowcaseGimmick();

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

	float lerpTime;

	FTimerHandle falloverT;

	// �������� ��������.(��ó �����)
	void RedBull(AActor* ActivePlayer);
	// �������� ������ ��������� �����־���.(ȥ��)
	void Babo(AActor* ActivePlayer);
	// ������ ���̿� ���踦 ã�Ҵ�.(Ŭ����)
	void MasterKey(AActor* ActivePlayer);
};
