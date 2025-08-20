// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSelect.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_ITT_API UPlayerSelect : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* Point;
private:
	virtual void NativeConstruct() override;
};
