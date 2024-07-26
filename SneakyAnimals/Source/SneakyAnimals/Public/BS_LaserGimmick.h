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

	// ������� ���θ� ����( ȥ��)
	void ElectricGrill(AActor* ActivePlayer);
	// �溸����� ������ (��ü)
	void WarningDeath(AActor* ActivePlayer);
	// ��¥���� �׳ɳ�����ؼ� Ŭ����(Ŭ����)
	void Faker(AActor* ActivePlayer);	
};
