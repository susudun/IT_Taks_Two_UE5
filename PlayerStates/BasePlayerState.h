// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"
class UAbilitySystemComponent;
class UBaseAttributeSet;
class UBaseAbilitySystemComponent;
/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogPS, Log, All)
UCLASS()
class PROJECT_ITT_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABasePlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
#pragma region Inline
public:
	FORCEINLINE UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const { return BaseAbilitySystemComponent; }
	FORCEINLINE UBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet; }
#pragma endregion
#pragma region Variables
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "GAS")
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;
public:
	UPROPERTY(Replicated)
	int32 CharaterNumber=3;
#pragma endregion
};
