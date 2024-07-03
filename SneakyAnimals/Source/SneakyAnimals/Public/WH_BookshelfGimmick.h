// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "WH_BookshelfGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API AWH_BookshelfGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	AWH_BookshelfGimmick();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// ����
	// �θ� Ŭ�������� ����� UFUNCTION�� �ڽ� Ŭ�������� �������� ���� �ڽ� Ŭ�������� UFUNCTION ��ũ�θ� �ٽ� ������ �ʿ䰡 ����
	virtual UItemObject* GetDefaultItemObject() override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AFlyBook> flyBook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Myactivetype;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector activeRange;



private:
	
	float lerpTime;

	FTimerHandle falloverT;

	// å���� �Ѿ����� å�� ���� ��� ���
	void FallOver();
	// å�� ���ƴٴϴٰ� ���� �Ѹ� ���
	void BookCanFly();
	// å�� ��ư�̿��� ���� ����
	void ButtonBook();

	/*UFUNCTION()
	virtual void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	
};
