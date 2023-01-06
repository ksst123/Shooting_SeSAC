// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(50.0f, 300.0f, 150.0f));
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetCollisionProfileName(TEXT("BossPreset"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(1.0f, 6.0f, 3.0f));
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -150.0f));
	MeshComponent->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	TargetLocation = FVector(0.0f, 0.0f, 200.0f);

	/*FTimerHandle Pattern1Timer;
	GetWorld()->GetTimerManager().SetTimer(Pattern1Timer, this, &ABossEnemy::BossAttack1, 5, true, 3);*/
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GetActorLocation().Z >= 200.0f)
	//{
	//	// ���� ��ġ���� (0.0f, 0.0f, 200.0f) ��ġ���� �̵�
	//	SetActorLocation(GetActorLocation() - FVector(0.0f, 0.0f, DeltaTime * 100));


	//}

	// ���� ��ġ���� (0.0f, 0.0f, 200.0f) ��ġ���� �̵�
	//float distance = (StartLocation - GetActorLocation()).Length();
	//float shaking = FMath::RandRange(-1.0f, 1.0f);
	//if (distance <= 500.0f)
	//{
	//	FVector direction = GetActorUpVector() * -1;
	//	direction.Y = shaking;
	//	SetActorLocation(GetActorLocation() + direction * 200.0f * DeltaTime);
	//}
	//else
	//{
	//	// ���ϴ� ��ġ���� ������ ���� ��� ����
	//	SetActorLocation(FVector(0.0f, 0.0f, 200.0f));
	//}


	// ���� ��ġ���� (0.0f, 0.0f, 200.0f) ��ġ���� �̵�(���� ����)
	TestValue += DeltaTime * 0.5;
	
	// TestValue�� � ���� �ȿ��� ����� ���ϵ���(Clamp) �Ѵ�
	TestValue = FMath::Clamp(TestValue, 0.0f, 1.0f);

	FVector Result = FMath::Lerp(StartLocation, TargetLocation, TestValue);
	SetActorLocation(Result);

	if (CurrentTime >= PatternDelay)
	{
		BossAttack1(15.0f, 10);
		CurrentTime = 0;
	}
	else
	{
		CurrentTime += DeltaTime;
	}
}
	
void ABossEnemy::BossAttack1(float angle, int32 count) {

	// ���� �̿�
	// angle �������� count �� ��ŭ �Ѿ��� ����
	/*BulletSpawnPostion = GetActorLocation() + GetActorUpVector() * -100.0f;
	float ExtraBossBulletAngle = (count - 1) * angle * -0.5f;
	FRotator BulletSpawnRotation = FRotator(-90.0f, 0.0f, 0.0f);
	for (int32 i = 0; i < count; i++)
	{
		AEnemyBullet* EnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(BossBullet, BulletSpawnPostion, BulletSpawnRotation);
		FRotator AngleOffset = FRotator(0.0f, 0.0f, ExtraBossBulletAngle + angle * i);

		if (EnemyBullet != nullptr)
		{
			EnemyBullet->AddActorWorldRotation(AngleOffset);
		}
	}*/



	// ���� �̿�
	// angle �������� count �� ��ŭ �Ѿ��� ����
	// ���� ���� = 270 - (angle * (count - 1) * 0.5
	float StartAngle = 270 - (angle * (count - 1) * 0.5f);

	for (int32 i = 0; i < count; i++)
	{
		// �⺻ ��ǥ (0, rcos@, rsin@)
		FVector BulletBase = FVector(0.0f, 
			200 * FMath::Cos(FMath::DegreesToRadians(StartAngle + angle * i)), 
			200 * FMath::Sin(FMath::DegreesToRadians(StartAngle + angle * i)));

		AEnemyBullet* EnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(BossBullet, GetActorLocation() + BulletBase, FRotator(-90.0f, 0.0f, 0.0f));
		EnemyBullet->SetDirection(BulletBase.GetSafeNormal());
		// EnemyBullet->AddActorLocalRotation(FRotator(0.0f, StartAngle + angle * i, 0.0f));


		//AEnemyBullet* EnemyBullet = GetWorld()->SpawnActor<AEnemyBullet>(BossBullet, GetActorLocation() + BulletBase, FRotator(-90.0f, 0.0f, 0.0f));
		//// UpVector ���� BulletBase �������� ȸ�������� �� Rotator ���� ���
		FRotator rot = UKismetMathLibrary::MakeRotFromZX(EnemyBullet->GetActorUpVector(), BulletBase.GetSafeNormal());
		EnemyBullet->SetActorRotation(rot);
		//// EnemyBullet->SetDirection(EnemyBullet->GetActorForwardVector());
	}
}

