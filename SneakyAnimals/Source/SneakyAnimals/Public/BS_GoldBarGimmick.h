// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "BS_GoldBarGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ABS_GoldBarGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ABS_GoldBarGimmick();

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
	
	TArray<class ATestPlayer*> players;

	FTimerHandle goldT;

	class ATestPlayer* _target;

	// �մ�� �溸�︮�� �����Ѹ� ���� (�����Ѹ�)
	void Alarm(AActor* ActivePlayer);
	// ���� �̲������� �ݱ��� �¾Ҵ�.(ȥ��)
	void Golden(AActor* ActivePlayer);
	// �ݱ��� ��� ����ġ���� ��� �� ���� Ż��(Ŭ����)
	void DoorOpen();

};
