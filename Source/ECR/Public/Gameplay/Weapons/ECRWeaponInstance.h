// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Equipment/ECREquipmentInstance.h"
#include "Cosmetics/ECRCosmeticAnimationTypes.h"
#include "Cosmetics/ECRPawnComponent_CharacterParts.h"
#include "Gameplay/GAS/ECRAbilitySourceInterface.h"
#include "ECRWeaponInstance.generated.h"

/**
 * UECRWeaponInstance
 *
 * A piece of equipment representing a weapon spawned and applied to a pawn
 */
UCLASS()
class UECRWeaponInstance : public UECREquipmentInstance, public IECRAbilitySourceInterface
{
	GENERATED_BODY()

public:
	UECRWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UECREquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	//~End of UECREquipmentInstance interface

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

	// Returns how long it's been since the weapon was interacted with (fired or equipped)
	UFUNCTION(BlueprintPure)
	float GetTimeSinceLastInteractedWith() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Animation)
	UAnimMontage* GetSwitchToExecutionMontage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Animation)
	UAnimMontage* GetKillerExecutionMontage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Animation)
	UAnimMontage* GetVictimExecutionMontage(AActor* TargetActor) const;

	void LinkAnimLayer() const;

	//~IECRAbilitySourceInterface interface
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr,
	                                     const FGameplayTagContainer* TargetTags = nullptr) const override;
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial,
	                                             const FGameplayTagContainer* SourceTags = nullptr,
	                                             const FGameplayTagContainer* TargetTags = nullptr) const override;
	virtual float GetArmorPenetration() const override;
	//~End of IECRAbilitySourceInterface interface
protected:
	// Choose the best layer from EquippedAnimSet or UnequippedAnimSet based on the specified gameplay tags
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(const FGameplayTagContainer& CosmeticTags) const;

	UAnimMontage* GetMontageFromSet(const FECRAnimMontageSelectionSet& SelectionSet, AActor* TargetActor) const;

	void LoadMontages();

	UFUNCTION()
	void OnCharacterPartsChanged(UECRPawnComponent_CharacterParts* ComponentWithChangedParts);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FECRAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FECRAnimLayerSelectionSet UnequippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FECRAnimMontageSelectionSet KillerExecutionMontageSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FECRAnimMontageSelectionSet VictimExecutionMontageSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FECRAnimMontageSelectionSet SwitchToMontageSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Damage)
	float ArmorPenetration;

	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
};
