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

	// 누르면 경비원이 와서 다죽음(전체)
	void DangerousGaurd(AActor* ActivePlayer);
	// 누르면 바닥이 열려서 떨어진다.(혼자)
	void DropMe(AActor* ActivePlayer);
	// 누르면 뒤로 날아간다.(혼자)
	void ThrowMe(AActor* ActivePlayer);
	// 경비원이 문을 열어주러 왔다.
	void SafeGaurd();
	
};
