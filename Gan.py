import random
import numpy as np
import pandas as pd
from tensorflow.keras.layers import Input, Dense, LeakyReLU
from tensorflow.keras.models import Sequential, Model

# Define ranges based on the provided data (with some variation)
alcohol_range = (278.000000, 460.000000)
methane_range = (304.000000, 442.000000)
temperature_range = (29.000000, 41.000000)
humidity_range = (38.000000, 65.000000)

# Define the GAN model
generator = Sequential([
    Dense(128, input_shape=(4,)),
    LeakyReLU(0.2),
    Dense(128, activation='relu'),
    Dense(4, activation='tanh')
])

discriminator = Sequential([
    Input(shape=(4,)),
    Dense(128, activation='relu'),
    LeakyReLU(0.2),
    Dense(128, activation='relu'),
    LeakyReLU(0.2),
    Dense(1, activation='sigmoid')
])

gan = Model(inputs=generator.input, outputs=discriminator(generator.output))
gan.compile(loss='binary_crossentropy', optimizer='adam')

num_samples = 500
num_epochs = 1000
batch_size = 32

for epoch in range(num_epochs):
    noise = np.random.normal(size=(batch_size, 4))
    generated_data = generator.predict(noise)

    real_data = np.random.uniform(size=(batch_size, 4))
    data = np.concatenate((real_data, generated_data))

    labels = np.zeros((batch_size, 1)) + 0.9
    labels_generated = np.zeros((batch_size, 1)) + 0.1
    labels = np.concatenate((labels, labels_generated))

    discriminator.compile(optimizer='adam', loss='binary_crossentropy')


    d_loss = discriminator.train_on_batch(data, labels)

    noise = np.random.normal(size=(batch_size, 4))
    g_loss = gan.train_on_batch(noise, np.zeros((batch_size, 1)))

    if epoch % 1000 == 0:
        print(f"Epoch: {epoch} \t Discriminator Loss: {d_loss} \t Generator Loss: {g_loss}")

num_synthetic_samples = 50
synthetic_data = generator.predict(np.random.normal(size=(num_synthetic_samples, 4)))


df = pd.DataFrame(synthetic_data, columns=['Alcohol', 'Methane', 'Temperature', 'Humidity'])
df['Condition'] = np.random.choice(['Spoiled', 'Unspoiled'], size=num_synthetic_samples)
df.to_csv('synthetic_data.csv', index=False)