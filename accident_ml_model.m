% ================= SETTINGS =================
channelID = 3349962;
readAPIKey = 'IDR8DUAZY02P5I0Z';
writeAPIKey = 'B11O5ENONN6BWYQO';

% ================= READ DATA =================
data = thingSpeakRead(channelID, ...
    'Fields', [1 2 3 4], ...
    'NumPoints', 200, ...
    'ReadKey', readAPIKey);

if isempty(data)
    error('No data retrieved from ThingSpeak! Make sure your channel has data.');
end

% ================= CLEAN DATA =================
valid = all(~isnan(data), 2);
data = data(valid, :);

if size(data, 1) < 5
    error(['Only ' num2str(size(data,1)) ' valid samples. Need at least 5 for training!']);
end

X = data(:,1:3);   % AccX, AccY, AccZ
Y = data(:,4);     % Label (0/1)

fprintf('Using %d data points for training\n', size(data, 1));
fprintf('Normal (0): %d samples\n', sum(Y == 0));
fprintf('Accident (1): %d samples\n', sum(Y == 1));

% ================= TRAIN MODEL =================
model = fitclinear(X, Y, 'Learner', 'logistic');

% ================= GET LATEST VALUE =================
latest = X(end, :);
AccX = latest(1);
AccY = latest(2);
AccZ = latest(3);

% ================= PREDICT =================
prediction = predict(model, latest);

% ================= DISPLAY OUTPUT =================
fprintf('----------------------------\n');
fprintf('AccX: %.2f\n', AccX);
fprintf('AccY: %.2f\n', AccY);
fprintf('AccZ: %.2f\n', AccZ);

if prediction == 1
    fprintf('🚨 ACCIDENT DETECTED (ML)\n');
else
    fprintf('✅ NORMAL (ML)\n');
end
fprintf('----------------------------\n');

% ================= WRITE BACK TO THINGSPEAK (Optional) =================
% pause(16);
% thingSpeakWrite(channelID, ...
%     'Fields', 5, ...
%     'Values', prediction, ...
%     'WriteKey', writeAPIKey);
% fprintf('Prediction written to Field 5\n');
