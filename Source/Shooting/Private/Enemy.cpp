// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingFlight.h"
#include "EngineUtils.h"
#include "ShootingGameModeBase.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(50.0f));
	// Collision Enabled ���� Query and Physics �� ����
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// Object Type(Collision Response Channel?) �� "Enemy"�� ����
	BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
	// response channel �� �ϰ������� ignore ���·� ó��
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	// response channel �� Player �� Bullet ä�ο� ���ؼ��� Overlap ���� ó��
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// ���� ����� EnemyArray �迭�� �ڱ� �ڽ��� �ִ´�(����Ű�� �Ѵ�).
	AShootingGameModeBase* MyGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	if (MyGM != nullptr)
	{
		MyGM->EnemyArray.Add(this);
	}

	// �÷��̾� ���� �Լ� ����
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	// ������ �̺�Ʈ Ȱ��ȭ
	BoxComponent->SetGenerateOverlapEvents(true);



	// �������� ���� ���� �Ǵ� �÷��̾� �������� �̵� ����
	// ���� Ȯ���� ������ �̿��ؼ� 7:3(����:�÷��̾�) ����
	int32 RandomNum = FMath::RandRange(1, 100);

	// ���� ���� TraceRate ���϶�� �÷��̾ ���ؼ� (�÷��̾� ��ġ - ���� ��ġ = �� ����)
	if (RandomNum <= TraceRate)		
	{
		//// ���忡�� Ư���� ��ü�� ã�� 1��° ���.
		//AActor* TraceTarget = UGameplayStatics::GetActorOfClass(GetWorld(), AShootingFlight::StaticClass());
		//AShootingFlight* RealTraceTarget = Cast<AShootingFlight>(TraceTarget);
		/*if (RealTraceTarget != nullptr)
		{
			float temp = RealTraceTarget.MoveSpeed;
		}*/

		// ���忡�� Ư���� ��ü�� ã�� 2��° ���
		// iterator ������ itr; �� itr != nullptr;  ���ǽİ� ����
		// 1��° ����� � ������ Ŭ������ ���� �������� ��ȯ�ϱ� ������ ĳ������ ����� �ϴµ�, 2��°�� ĳ������ �ʿ����.

		for (TActorIterator<AShootingFlight> itr(GetWorld()); itr; ++itr)
		{
			TraceTarget = *itr;
		}
		// float temp = TraceTarget->MoveSpeed;

		if (TraceTarget != nullptr)
		{
			// �÷��̾� ��ġ - ���� ��ġ = �� ����
			FVector TargetDirection = TraceTarget->GetActorLocation() - GetActorLocation();
			// TargetDirection �� ���̰� �� �� ���¿��� MoveSpeed�� ���ϸ� �ʹ� Ŀ���Ƿ� ����ȭ�ؼ� 1¥�� �������ͷ� �����.
			TargetDirection.Normalize();
			MoveDirection = TargetDirection;
		}
	}
	else // �׷��� ������ ���� ����
	{
		MoveDirection = GetActorForwardVector();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		// MoveDirection = GetActorForwardVector();
		FVector EnemyMove = GetActorLocation() + MoveSpeed * DeltaTime * MoveDirection;
		SetActorLocation(EnemyMove);
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// OtherActor �� ShootingFlight ���� Ȯ��
	AShootingFlight* PlayerFlight = Cast<AShootingFlight>(OtherActor);
	// ���� OtherActor �� ShootingFlight ���
	if (PlayerFlight != nullptr)
	{
		// �÷��̾� ����
		PlayerFlight->Destroy();

		// MenuWidget�� �����ϴ� �Լ��� ����
		AShootingGameModeBase* GameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode != nullptr)
		{
			GameMode->ShowMenu();
		}

		// �÷��̾�� �浹 �� �÷��̾� �� ����
		// PlayerFlight->ReserveChangeColor(0.1f);

		// �����ε� ����
		Destroy();
	}
}

void AEnemy::DestroyMyself() {
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), true);
	Destroy();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	// �ڱ� �ڽ��� �迭���� ����
	AShootingGameModeBase* MyGM = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	if (MyGM != nullptr)
	{
		MyGM->EnemyArray.Remove(this);
	}
}

