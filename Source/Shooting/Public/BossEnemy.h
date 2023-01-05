// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEnemy.generated.h"

UCLASS()
class SHOOTING_API ABossEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	float TestValue;

private:
	FVector StartLocation;
	FVector TargetLocation;
};
