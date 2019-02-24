// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaunchPad();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LunchStrength;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LunchAngle;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
		UParticleSystem* LaunchFX;

	UFUNCTION()
		void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
