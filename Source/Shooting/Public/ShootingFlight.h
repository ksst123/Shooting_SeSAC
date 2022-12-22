// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "ShootingFlight.generated.h"

UCLASS()
class SHOOTING_API AShootingFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShootingFlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player Settings")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player Settings")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Settings")
	float MoveSpeed = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Settings")
	TSubclassOf<class ABullet> BulletFactory;
	// 블루프린트, 머티리얼, 외부에서 만들어진 파일들은 TSubclassOf 통해서 참조

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_MoveHorizontal;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_MoveVertical;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputAction* IA_FireBullet;

	UPROPERTY(EditDefaultsOnly, Category = "Player Settings")
	class UInputMappingContext* IMC_MyMapping;

private:
	// legacy input 방식 함수들
	/*UFUNCTION(BlueprintCallable)
	void MoveHorizontal(float value);

	UFUNCTION(BlueprintCallable)
	void MoveVertical(float value);

	UFUNCTION(BlueprintCallable)
	void FireBullet();*/

	// enhanced input 방식 함수들
	UFUNCTION(BlueprintCallable)
	void MoveHorizontal(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void MoveVertical(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable)
	void FireBullet();

	float h, v;
	FVector direction;
};
