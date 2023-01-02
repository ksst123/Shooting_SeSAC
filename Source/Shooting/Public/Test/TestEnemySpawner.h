// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestEnemySpawner.generated.h"

UCLASS()
class SHOOTING_API ATestEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	float SpawnTime = 2.0f;

	UPROPERTY(EditAnywhere, Category="Spawner Settings")
	class USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	class UArrowComponent* SpawnerDirection;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings")
	TSubclassOf<class ATestEnemyActor> EnemyFactory;

private:
	float CurrentTime = 0.0f;
};
