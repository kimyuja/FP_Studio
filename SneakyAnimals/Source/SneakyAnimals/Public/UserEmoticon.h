// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserEmoticon.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	COW				UMETA(DisplayName = "Cow Character"),
	FROG			UMETA(DisplayName = "Frog Character"),
	RABBIT			UMETA(DisplayName = "Rabbit Character"),
	TURTLE			UMETA(DisplayName = "Turtle Character"),
}; 



UCLASS()
class SNEAKYANIMALS_API UUserEmoticon : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	FTimerHandle closeT;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* curEmoticon;

	UPROPERTY(EditAnywhere, Transient ,meta = (BindWidgetAnim))
	class UWidgetAnimation* EmoticonAnim;

	UPROPERTY(EditAnywhere)
	TArray<class UTexture2D*> cowEmoticons;

	UPROPERTY(EditAnywhere)
	TArray<class UTexture2D*> frogEmoticons;
	
	UPROPERTY(EditAnywhere)
	TArray<class UTexture2D*> rabbitEmoticons;
	
	UPROPERTY(EditAnywhere)
	TArray<class UTexture2D*> turtleEmoticons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerType playerType = EPlayerType::COW;

	UFUNCTION()
	void ShowEmoticon(int32 emoNum);

};
