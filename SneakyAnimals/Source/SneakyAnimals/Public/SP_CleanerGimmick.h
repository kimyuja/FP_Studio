// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick.h"
#include "SP_CleanerGimmick.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ASP_CleanerGimmick : public AGimmick
{
	GENERATED_BODY()

public:

	ASP_CleanerGimmick();

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

	// 음료수병에 담겨있던 청소약을 마셨다(혼자)
	void RedBull(AActor* ActivePlayer);
	// 청소도구에 발을 찧었다(혼자)
	void Lego(AActor* ActivePlayer);
	// 청소부가 사용하는 마스터키를 찾았다.(클리어)
	void MasterKey(AActor* ActivePlayer);
};
