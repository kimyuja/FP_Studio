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

	// 선민
	// 부모 클래스에서 선언된 UFUNCTION을 자식 클래스에서 재정의할 때는 자식 클래스에서 UFUNCTION 매크로를 다시 선언할 필요가 없음
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

	// 책장이 넘어져서 책장 주위 사람 사망
	void FallOver();
	// 책이 날아다니다가 랜덤 한명 사망
	void BookCanFly();
	// 책이 버튼이여서 문이 열림
	void ButtonBook();

	/*UFUNCTION()
	virtual void SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	
};
