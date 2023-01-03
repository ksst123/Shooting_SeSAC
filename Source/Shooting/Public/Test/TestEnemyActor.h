// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestEnemyActor.generated.h"

UCLASS()
class SHOOTING_API ATestEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Test Settings")
	float MoveSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Test Settings")
	class UStaticMeshComponent* MeshComponent;

private:
	FVector MoveDirection;
};
